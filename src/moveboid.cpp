#include "boid.hpp"
#include "constants.hpp"

struct CLData {
	cl::Context ctx;
	cl::CommandQueue q;
	std::map<std::string, cl::Kernel> k;
} data;

void oclInit() {
	cl::Program program;
	try {
		FILE* f = fopen("boids.cl", "r");
		assert(f);
		fseek(f, 0, SEEK_END);
		long long size = ftell(f);
		fseek(f, 0, SEEK_SET);

		std::string str(size, 0);
		fread(&str[0], 1, size, f);
		fclose(f);

		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		assert(platforms.size());
		cl_context_properties properties[] =
			{CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[0](), 0};

		data.ctx = cl::Context(CL_DEVICE_TYPE_GPU, properties);
		data.q = cl::CommandQueue(data.ctx, data.ctx.getInfo<CL_CONTEXT_DEVICES>()[0]);

		cl::Program::Sources source;
		source.push_back(std::make_pair(str.c_str(), str.size()));

		program = cl::Program(data.ctx, source);
		program.build(std::vector<cl::Device>());

		std::vector<cl::Kernel> k;
		program.createKernels(&k);
		for (int i = 0; i < k.size(); ++i)
			data.k[k[i].getInfo<CL_KERNEL_FUNCTION_NAME>()] = k[i];

	}  catch (const std::exception& e) {
		printf("initOCL: %s\n", e.what());
	}
	printf("LOG:%s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>
		(program.getInfo<CL_PROGRAM_DEVICES>()[0]).c_str());
}

void qtize(Vec v, int& x, int &y) {
	x = int((v.x-low.x)/SEE);
	y = int((v.y-low.y)/SEE);
}

void moveBoids(float time) {
	int sx, sy, x, y;
	qtize(high, sx, sy);
	sx++; sy++;

	int *bt = new int[sx*sy];
	for (int i = 0; i < sx*sy; ++i) bt[i] = -1;
	for (int i = 0; i < bs.size(); ++i) {
		bs[i].next = -1;
		qtize(bs[i].p, x, y);
		assert(x >= 0 && y >= 0 && x < sx && y < sy);
		bs[i].next = bt[x*sx + y];
		bt[x*sx + y] = i;
	}

	cl::Buffer inbuf(data.ctx, CL_MEM_READ_WRITE, sizeof(Boid)*bs.size());
	cl::Buffer btbuf(data.ctx, CL_MEM_READ_WRITE, sizeof(int)*sx*sy);
	cl::Buffer outbuf(data.ctx, CL_MEM_READ_WRITE, sizeof(Boid)*bs.size());
	data.q.enqueueWriteBuffer(inbuf, true, 0, sizeof(Boid)*bs.size(), &bs[0]);
	data.q.enqueueWriteBuffer(btbuf, true, 0, sizeof(int)*sx*sy, bt);
	data.k["move"].setArg(0, inbuf);
	data.k["move"].setArg(1, outbuf);
	data.k["move"].setArg(2, btbuf);
	data.k["move"].setArg(3, time);
	data.k["move"].setArg(4, low);
	data.k["move"].setArg(5, high);
	data.q.finish();
	data.q.enqueueNDRangeKernel(data.k["move"], cl::NullRange, cl::NDRange(bs.size()), cl::NullRange);
	data.q.finish();
	data.q.enqueueReadBuffer(outbuf, true, 0, sizeof(Boid)*bs.size(), &bs[0]);
	data.q.finish();

	delete[] bt;
}

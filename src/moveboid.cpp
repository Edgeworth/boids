#include "boid.hpp"
#include "constants.hpp"

struct CLData {
  cl::Context ctx;
  cl::CommandQueue q;
  cl::Kernel k;
} data;

void oclInit() {
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
  if (platforms.size() == 0) {
    printf("No available platforms.\n");
    std::exit(1);
  }
  for (int i = 0; i < platforms.size(); ++i) {
    printf("Platform %d: %s\n", i, platforms[i].getInfo<CL_PLATFORM_NAME>().c_str());
  }

  auto& platform = platforms[0];
  printf("Using platform: %s\n", platform.getInfo<CL_PLATFORM_NAME>().c_str());

  std::vector<cl::Device> devices;
  platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
  if (devices.size() == 0) {
    printf("No available devices.\n");
    std::exit(1);
  }
  for (int i = 0; i < devices.size(); ++i) {
    printf("Device %d: %s\n", i, devices[i].getInfo<CL_DEVICE_NAME>().c_str());
  }

  auto dev = devices[0];
  data.ctx = cl::Context({devices});
  printf("Using device: %s\n", dev.getInfo<CL_DEVICE_NAME>().c_str());

  data.q = cl::CommandQueue(data.ctx, dev);

  cl::Program::Sources source;
  source.push_back(std::make_pair(str.c_str(), str.length()));
  cl::Program program = cl::Program(data.ctx, source);

  try {
    program.build({dev});
  }  catch (const cl::Error& e) {
    printf("initOCL: %s, code: %d\n", e.what(), int(e.err()));
  }

  printf("LOG:%s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>
    (program.getInfo<CL_PROGRAM_DEVICES>()[0]).c_str());
  data.k = cl::Kernel(program, "move");
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
  try {
    cl::Buffer inbuf(data.ctx, CL_MEM_READ_WRITE, sizeof(Boid)*bs.size());
    cl::Buffer btbuf(data.ctx, CL_MEM_READ_WRITE, sizeof(int)*sx*sy);
    cl::Buffer outbuf(data.ctx, CL_MEM_READ_WRITE, sizeof(Boid)*bs.size());
    data.q.enqueueWriteBuffer(inbuf, CL_TRUE, 0, sizeof(Boid)*bs.size(), &bs[0]);
    data.q.enqueueWriteBuffer(btbuf, CL_TRUE, 0, sizeof(int)*sx*sy, bt);
    data.k.setArg(0, inbuf);
    data.k.setArg(1, outbuf);
    data.k.setArg(2, btbuf);
    data.k.setArg(3, time);
    data.k.setArg(4, low);
    data.k.setArg(5, high);
    data.q.finish();
    data.q.enqueueNDRangeKernel(data.k, cl::NullRange, cl::NDRange(bs.size()), cl::NullRange);
    data.q.finish();
    data.q.enqueueReadBuffer(outbuf, CL_TRUE, 0, sizeof(Boid)*bs.size(), &bs[0]);
    data.q.finish();
  } catch (const cl::Error& e) {
    printf("moveBoids: %s, code: %d\n", e.what(), int(e.err()));
    throw e;
  }

  delete[] bt;
}

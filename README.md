# Ray Tracing in C

This is my implementation of the book [RayTracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

The original book is in C++ but my version is in plain C.

This code was only tested in `AppleClang`, `gcc` might also work but I didn't tested.

To build and run:

```bash
git clone https://github.com/pnhuy/myraytracer
cd myraytracer
cmake -B build
cmake --build build
./build/main > ./build/image.ppm
```

The image format `.ppm` can be viewed natively in MacOS, but might not be opened in other system. In the latter case, you can view it online, such as [PPM Viewer](https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html).

The below is a demo result.

![Ray Tracing Sample](https://raytracing.github.io/images/img-1.23-book1-final.jpg)

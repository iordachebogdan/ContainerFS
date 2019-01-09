# ContainerFS
ZIP filesystem using libfuse and libzip

## Requirements
- [libfuse3](https://github.com/libfuse/libfuse)
- [libzip](https://github.com/nih-at/libzip)

## Compiling
```
mkdir -p build
cd build
cmake ..
make
```

## Roadmap
- [x] getattr
- [x] readlink
- [x] open
- [x] read
- [x] readdir
- [ ] release
- [x] init

## Testing
- [ ] Zips with deep directory structure
- [ ] Zips with many files
- [ ] Zips with big files

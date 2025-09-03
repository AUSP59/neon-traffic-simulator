.PHONY: build debug test asan package clean
build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j
debug:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build -j
test:
	ctest --test-dir build --output-on-failure
asan:
	cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" -DCMAKE_LINKER_FLAGS="-fsanitize=address,undefined"
	cmake --build build -j
package:
	cpack
clean:
	rm -rf build dist

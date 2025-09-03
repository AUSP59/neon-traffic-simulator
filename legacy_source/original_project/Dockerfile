# syntax=docker/dockerfile:1
FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends     build-essential cmake ninja-build git ca-certificates && rm -rf /var/lib/apt/lists/*
WORKDIR /work
COPY . .
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build -j

FROM ubuntu:24.04 AS run
WORKDIR /work
COPY --from=build /work/build /work/build
ENTRYPOINT ["/work/build/bin/neon_traffic_cli", "--version"]

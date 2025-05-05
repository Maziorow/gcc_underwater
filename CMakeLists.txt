cmake_minimum_required(VERSION 3.20)
project(DeviceSim C)

set(CMAKE_C_STANDARD 99)

find_library(sndfile libsndfile)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(DeviceSim main.c
                        src/convert.c
                        src/dsp.c
                        src/fft.c)

target_include_directories(DeviceSim PRIVATE ${SNDFILE_INCLUDE_DIRS})

target_link_libraries(DeviceSim
                        PRIVATE m libIp_xfft_v9_1_bitacc_cmodel.so
                        PRIVATE ${SNDFILE_LIBRARIES}
                    )
                    


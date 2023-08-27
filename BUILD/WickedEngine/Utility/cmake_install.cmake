# Install script for directory: /home/deck/Crucible/WickedEngine/Utility

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/Utility/DirectXCollision.h"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXColors.h"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXMath.h"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXMathCommon.h"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXPackedVector.h"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXCollision.inl"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXMathConvert.inl"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXMathMatrix.inl"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXMathMisc.inl"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXMathVector.inl"
    "/home/deck/Crucible/WickedEngine/Utility/DirectXPackedVector.inl"
    "/home/deck/Crucible/WickedEngine/Utility/liberation_sans.h"
    "/home/deck/Crucible/WickedEngine/Utility/portable-file-dialogs.h"
    "/home/deck/Crucible/WickedEngine/Utility/sal.h"
    "/home/deck/Crucible/WickedEngine/Utility/tinyddsloader.h"
    "/home/deck/Crucible/WickedEngine/Utility/robin_hood.h"
    "/home/deck/Crucible/WickedEngine/Utility/spirv_reflect.h"
    "/home/deck/Crucible/WickedEngine/Utility/stb_image.h"
    "/home/deck/Crucible/WickedEngine/Utility/stb_image_write.h"
    "/home/deck/Crucible/WickedEngine/Utility/stb_rect_pack.h"
    "/home/deck/Crucible/WickedEngine/Utility/stb_truetype.h"
    "/home/deck/Crucible/WickedEngine/Utility/qoi.h"
    "/home/deck/Crucible/WickedEngine/Utility/dxcapi.h"
    "/home/deck/Crucible/WickedEngine/Utility/D3D12MemAlloc.h"
    "/home/deck/Crucible/WickedEngine/Utility/volk.h"
    "/home/deck/Crucible/WickedEngine/Utility/vk_mem_alloc.h"
    "/home/deck/Crucible/WickedEngine/Utility/flat_hash_map.hpp"
    "/home/deck/Crucible/WickedEngine/Utility/pugixml.hpp"
    "/home/deck/Crucible/WickedEngine/Utility/pugiconfig.hpp"
    "/home/deck/Crucible/WickedEngine/Utility/h264.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/dx12" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3d12.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3d12compatibility.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3d12sdklayers.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dcommon.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/dxgicommon.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/dxgiformat.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3d12shader.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3d12video.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_barriers.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_check_feature_support.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_core.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_default.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_pipeline_state_stream.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_render_pass.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_resource_helpers.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_root_signature.h"
    "/home/deck/Crucible/WickedEngine/Utility/dx12/d3dx12_state_object.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/include/spirv/unified1" TYPE FILE FILES "/home/deck/Crucible/WickedEngine/Utility/include/spirv/unified1/spirv.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/vulkan" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_layer.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_sdk_platform.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_directfb.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_ggp.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_ios.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_macos.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_metal.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_screen.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_vi.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_wayland.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_win32.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_xcb.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_xlib.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_xlib_xrandr.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_icd.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_platform.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_android.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_beta.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_core.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_fuchsia.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/vulkan/vk_video" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_layer.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_sdk_platform.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_directfb.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_ggp.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_ios.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_macos.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_metal.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_screen.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_vi.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_wayland.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_win32.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_xcb.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_xlib.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_xlib_xrandr.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_icd.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vk_platform.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_android.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_beta.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_core.h"
    "/home/deck/Crucible/WickedEngine/Utility/vulkan/vulkan_fuchsia.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/basis_universal/encoder" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/apg_bmp.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_astc_decomp.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_backend.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_basis_file.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_bc7enc.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_comp.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_enc.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_etc.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_frontend.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_global_selector_palette_helpers.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_gpu_texture.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_kernels_declares.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_kernels_imp.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_miniz.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_pvrtc1_4.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_resampler.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_resampler_filters.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_ssim.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/basisu_uastc_enc.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/cppspmd_flow.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/cppspmd_math.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/cppspmd_math_declares.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/cppspmd_sse.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/cppspmd_type_aliases.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/jpgd.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/encoder/lodepng.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/basis_universal/transcoder" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_containers.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_containers_impl.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_file_headers.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_global_selector_cb.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_global_selector_palette.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_transcoder.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_transcoder_internal.h"
    "/home/deck/Crucible/WickedEngine/Utility/basis_universal/transcoder/basisu_transcoder_uastc.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/basis_universal/zstd" TYPE FILE FILES "/home/deck/Crucible/WickedEngine/Utility/basis_universal/zstd/zstd.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/Utility/dxc/Support" TYPE FILE FILES "/home/deck/Crucible/WickedEngine/Utility/dxc/Support/WinAdapter.h")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/deck/Crucible/BUILD/WickedEngine/Utility/FAudio/cmake_install.cmake")

endif()


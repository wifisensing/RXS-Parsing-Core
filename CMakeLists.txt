include_directories(preprocessor/)
file(GLOB modular_ps_core_source *.cxx preprocessor/generated/*.cpp)
ADD_LIBRARY(modular_parsing_core SHARED ${modular_ps_core_source})
install(TARGETS modular_parsing_core DESTINATION ${INSTALL_LIBRARY_DIR})
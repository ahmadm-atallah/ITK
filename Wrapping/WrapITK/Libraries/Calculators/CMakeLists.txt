WRAP_LIBRARY(Calculators)

set(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base Numerics)

set(WRAPPER_LIBRARY_LINK_LIBRARIES ${ITK_LIBRARIES})

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()

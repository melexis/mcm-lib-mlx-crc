idf_component_register(
    SRCS "src/mlx_crc.c"
         "src/ganymede_kf_crc.c"
         "src/ganymede_xfe_crc.c"
    PRIV_INCLUDE_DIRS "src"
    INCLUDE_DIRS "include"
    REQUIRES
)

project('dpdk-tutorial', 'c')

## option process ##
enable_subs = get_option('enable_subs')

## build dpdk ##
enable_apps = 'pdump,proc-info,test-pmd'

disable_dirvers = 'bus/dpaa,bus/ifpga,common/cnxk,compress/isal,compress/octeontx,compress/zlib'
disable_dirvers = disable_dirvers + ',' + 'crypto/ccp,crypto/cnxk,crypto/dpaa_sec,crypto/openssl,crypto/aesni_mb,crypto/aesni_gcm,crypto/kasumi'
disable_dirvers = disable_dirvers + ',' + 'crypto/snow3g,crypto/zuc,mempool/dpaa,mempool/cnxk,net/af_packet,net/bnx2x,net/bonding,net/cnxk'
disable_dirvers = disable_dirvers + ',' + 'net/ipn3ke,net/liquidio,net/pcap,net/pfe,net/sfc,net/softnic,net/thunderx,raw/ifpga,net/af_xdp'

dpdk_defaule_arg = 'disable_drivers=' + disable_dirvers
dpdk_defaule_arg = dpdk_defaule_arg + ',' + 'enable_apps=' + enable_apps
dpdk_defaule_arg = dpdk_defaule_arg + ',' + 'default_library=' + 'static'

subproject('dpdk', default_options: dpdk_defaule_arg)
libdpdk_proj = subproject('dpdk')

libdpdk_dep = libdpdk_proj.get_variable('libdpdk_dep')

foreach sub : enable_subs
    subdir(sub)
endforeach
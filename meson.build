project('dpdk-tutorial', 'c')

## option process ##
enable_subs = get_option('enable_subs')

## build dpdk ##
dpdk_enable_apps = 'enable_apps=pdump,proc-info,test-pmd'

dpdk_disable_dirvers = 'disable_drivers=bus/dpaa,bus/ifpga,common/cnxk,compress/isal,compress/octeontx,compress/zlib'
dpdk_disable_dirvers = dpdk_disable_dirvers + ',' + 'crypto/ccp,crypto/cnxk,crypto/dpaa_sec,crypto/openssl,crypto/aesni_mb,crypto/aesni_gcm,crypto/kasumi'
dpdk_disable_dirvers = dpdk_disable_dirvers + ',' + 'crypto/snow3g,crypto/zuc,mempool/dpaa,mempool/cnxk,net/af_packet,net/bnx2x,net/bonding,net/cnxk'
dpdk_disable_dirvers = dpdk_disable_dirvers + ',' + 'net/ipn3ke,net/liquidio,net/pcap,net/pfe,net/sfc,net/softnic,net/thunderx,raw/ifpga,net/af_xdp'

dpdk_default_library = 'default_library=static'

dpdk_defaule_arg = [dpdk_enable_apps, dpdk_disable_dirvers, dpdk_default_library]

subproject('dpdk', default_options: dpdk_defaule_arg)
libdpdk_proj = subproject('dpdk')

libdpdk_dep = libdpdk_proj.get_variable('libdpdk_dep')

foreach sub : enable_subs
    subdir(sub)
endforeach
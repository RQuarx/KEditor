platform_source_files = files()

if host_machine.system() == 'windows'
    platform_source_files += files(
        'w_mapped_file.cc',
    )
elif host_machine.system() == 'linux'
    platform_source_files += files(
        'l_mapped_file.cc',
    )
else
    error('unsupported system')
endif
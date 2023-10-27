import os
from conan import ConanFile
from conan.tools.gnu import Autotools, AutotoolsToolchain, AutotoolsDeps
from conan.tools.scm import Git
from conan.tools.files import save, load, copy

class UbloxParserConan(ConanFile):
    name = 'ublox_parser'
    author = 'A. Gagniere (antoine.gagniere@orolia2s.com)'
    url = 'https://github.com/Orolia2s/ublox_parser'
    license='proprietary'
    description = 'Subscribe to u-blox messages'
    topics = ('ublox', 'nmea', 'serial_port')
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'PkgConfigDeps'
    exports_sources = 'src/*', 'include/*', 'Makefile', 'README.md'
    requires = 'libft/2.10', 'log.c/cci.20200620'
    options = {
        'shared': [True, False],
        'fPIC': [True, False],
        'colored_logs': [True, False]
    }
    default_options = {
        'shared': False,
        'fPIC': False,
        'colored_logs': True
    }

    def _get_latest_tag(self):
        git = Git(self, folder=self.recipe_folder)
        return git.run('tag --sort "-version:refname" --merged').split('\n', 1)[0]

    def export(self):
        save(self, os.path.join(self.export_folder, 'version.txt'), self._get_latest_tag())

    def set_version(self):
        try:
            self.version = load(self, 'version.txt')
        except:
            self.version = self._get_latest_tag()

    def configure(self):
        self.settings.rm_safe('compiler.libcxx')
        self.settings.rm_safe('compiler.cppstd')
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def generate(self):
        autotools = AutotoolsDeps(self)
        autotools.environment.define('VERSION', self.version)
        autotools.generate()

        toolchain = AutotoolsToolchain(self)
        if self.options.colored_logs:
            toolchain.extra_defines.append('LOG_USE_COLOR')
        toolchain.generate()

    def build(self):
        autotools = Autotools(self)
        autotools.make('shared' if self.options.shared else 'static')
        autotools.make('clean')

    def package(self):
        copy(self, '*.h',
             os.path.join(self.source_folder, 'include'),
             os.path.join(self.package_folder, 'include'))
        for file in ('*.a', '*.so'):
            copy(self, file, self.build_folder,
                 os.path.join(self.package_folder, 'lib'))
        for file in ('version.txt', 'README.md'):
            copy(self, file, self.source_folder, self.package_folder)

    def package_info(self):
        self.cpp_info.libs = ['ublox_parser']

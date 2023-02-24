from conans import ConanFile
from conan.tools.gnu import Autotools, AutotoolsToolchain
from conan.tools.scm import Git

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

    def set_version(self):
        git = Git(self, folder=self.recipe_folder)
        self.version = git.run('tag --sort "-version:refname" --merged').split('\n', 1)[0]

    def configure(self):
        del self.settings.compiler.libcxx
        del self.settings.compiler.cppstd
        if self.options.shared:
            self.options.fPIC = True

    def generate(self):
        toolchain = AutotoolsToolchain(self)
        if self.options.colored_logs:
            toolchain.extra_defines.append('LOG_USE_COLOR')
        toolchain.generate()

    def build(self):
        autotools = Autotools(self)
        autotools.make('shared' if self.options.shared else 'static')
        autotools.make('clean')

    def package(self):
        self.copy('*.h', dst='include', src='include')
        self.copy('*.a', dst='lib', keep_path=False)
        self.copy('*.so', dst='lib', keep_path=False)
        self.copy('README.md')

    def package_info(self):
        self.cpp_info.libs = ['ublox_parser']

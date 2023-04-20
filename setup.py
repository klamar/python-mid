from distutils.core import setup, Extension

module = Extension('mid', sources=['midmodule.c'])

setup(name='python-mid',
      version='1.0.0',
      description='MID module',
      author='Martin Klapproth',
      url='https://github.com/klamar/python-mid',
      maintainer_email="martin.klapproth@makrotan.com",
      license="MIT",
      ext_modules=[module],
      classifiers=[
          'Development Status :: 5 - Production/Stable',
          'Intended Audience :: Developers',
          'License :: OSI Approved :: MIT License',
          'Operating System :: POSIX :: Linux',
          'Programming Language :: Python',
          'Programming Language :: Python :: 3',
          'Programming Language :: Python :: 3.7',
          'Programming Language :: Python :: 3.8',
          'Programming Language :: Python :: 3.9',
          'Programming Language :: Python :: 3.10',
      ],
      )

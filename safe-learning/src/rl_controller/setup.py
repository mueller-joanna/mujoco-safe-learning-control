from setuptools import find_packages, setup

package_name = 'rl_controller'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='koffi',
    maintainer_email='koffi.pot@posteo.de',
    description='TODO: Package description',
    license='wtfpl',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'talker = rl_controller.main:main',
        ],
    },
)

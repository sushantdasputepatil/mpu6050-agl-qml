How to add i2c-tools in AGL:
AGL uses RPM Package Manager (Red-hat Package Manager)
1.Check on http://layers.openembedded.org/layerindex/branch/master/layers/ is there any recipie of i2c-tools is present
2.If yes then In AGL workspace first Source the tools environment setup script by using a command like the following:
	source meta-agl/scripts/aglsetup.sh -m raspberrypi3 agl-demo agl-netboot agl-appfw-smack
3.Now Run the BitBake command to build the i2c-tools:
	bitbake i2c-tools
4.After successful build check folder ../agl-eel-5.1.0/build/tmp/deploy/rpm/
5.Here according to your sourced architecture that folder will be generated and inside that folder you will have your i2c-tools rpm files
6.Files generated as follows:
	i2c-tools-3.1.2-r0.armv7vehf_neon_vfpv4.rpm
	i2c-tools-dev-3.1.2-r0.armv7vehf_neon_vfpv4.rpm
	i2c-tools-dbg-3.1.2-r0.armv7vehf_neon_vfpv4.rpm
	i2c-tools-doc-3.1.2-r0.armv7vehf_neon_vfpv4.rpm
	i2c-tools-misc-3.1.2-r0.armv7vehf_neon_vfpv4.rpm
7.Now copy these files on your board containing AGL and install these rpm files using command:
	rpm -i i2c-tools*
(using of * installs all the files started with i2c-tools OR you can install individually as rpm -i i2c-tools-3.1.2-r0.armv7vehf_neon_vfpv4.rpm)
8.Now you can use various i2c-tools such as i2cdetect,i2cset,i2cget inside the AGL.
9.This method you can apply to add any components in AGL such as parted,e2fsprogs,96board-util,etc.
	


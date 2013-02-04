# script to copy binaries to <ssc>/sdk-release folder
cp -v linux_so/ssc64.so sdk-release/linux64/ssc64.so
cp -v dev/linux_wx3/SSCdev sdk-release/linux64/SSCdev
file sdk-release/linux64/sscdev64.bin > sdk-release/linux64/filetypes.txt
file sdk-release/linux64/ssc64.so >> sdk-release/linux64/filetypes.txt
echo "Dynamic library dependencies: compiled on Fedora 16" > sdk-release/linux64/dependencies.txt
echo "sscdev64.bin:" >> sdk-release/linux64/dependencies.txt
ldd sdk-release/linux64/sscdev64.bin >> sdk-release/linux64/dependencies.txt
echo "ssc64.so:" >> sdk-release/linux64/dependencies.txt
ldd sdk-release/linux64/ssc64.so >> sdk-release/linux64/dependencies.txt


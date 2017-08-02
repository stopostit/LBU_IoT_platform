export PATH=$PATH:/bin
sudo mkdir -p /mnt/eos
sudo mount /dev/sda5 /mnt/eos
sudo chroot /mnt/eos /bin/bash

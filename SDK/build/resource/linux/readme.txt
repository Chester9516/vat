
#if used systemd need rules file to enable usb privilege

put 'resource\udev\50-usb-vatek.rules' to '/etc/udev/rules.d/' and reboot to enabled.
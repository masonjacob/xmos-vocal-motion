# Attach XTAG Device to WSL and run UDEV rules

# Run the `usbipd wsl list` command and capture its output
$usbipdOutput = usbipd wsl list
# Use the Where-Object cmdlet to filter the list based on the DEVICE name
$filteredDevices = $usbipdOutput | Where-Object { $_ -match "XMOS XTAG-4" }
# Extract BUSID from the filtered list
$busids = $filteredDevices | ForEach-Object {
    # Split the line by whitespace and take the first element (BUSID)
    $busid = ($_ -split '\s+')[0]
    $busid
}
# Attach the busids to WSL
foreach ($busid in $busids) {
    usbipd wsl attach --busid $busid
}

# Add UDEV rules
$wslPath = wsl wslpath $PWD.Path 

wsl mkdir ~/scripts
wsl cp "$wslPath\99-xmos.rules" ~/scripts
wsl cp "$wslPath\setup_xmos_devices.sh" ~/scripts
wsl cp "$wslPath\check_xmos_devices.sh" ~/scripts
wsl chmod +x ~/scripts/setup_xmos_devices.sh
wsl chmod +x ~/scripts/check_xmos_devices.sh
wsl sudo -e ~/scripts/setup_xmos_devices.sh
wsl sudo -e ~/scripts/check_xmos_devices.sh

wsl sudo rm ~/scripts -r -f

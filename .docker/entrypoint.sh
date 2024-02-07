#!/bin/bash
# Basic entrypoint to change default terminal directory

# Add cd to .bashrc:
echo "cd /opt/xmos/sln_voice" >> ~/.bashrc


# Execute the command passed into this entrypoint
exec $@

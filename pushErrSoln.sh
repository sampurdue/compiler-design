#!/bin/sh
#if you get an error, execute the following lines

eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa

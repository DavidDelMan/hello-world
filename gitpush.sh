#!/bin/bash
# See http://osr600doc.sco.com/en/SHL_automate/_Passing_to_shell_script.html

# echo There are $# argument to $0: $*
# echo first argument: $1
# echo second argument: $2
# echo third argument: $3
# echo here they are again: $@

# function how_many {
#      echo "$# arguments were supplied."
# }
# how_many "$*"
# how_many "$@"

# Inputs:
#    1. branch name

git add .
read -p "Commit description: " desc
git commit -m "$desc"
git push origin master

# # verify correct branch is passed
# echo Branch name to push: $1

# # commit files
# echo Committing files 
# # 
# git push origin $1
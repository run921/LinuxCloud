# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions

#add by gao
export PS1='[\u]$PWD>'

#core文件大小
ulimit -c 10

#add by gao
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

#!bin/bash

alias jb=jobs
alias cdf='diff -Nupr'
alias st='git status'
alias gs='git status'
alias ga='git add'
alias gsh='git show'
alias gc='git commit'
alias gch='git checkout'
alias gd='git diff'
alias gk='git diff --check'
alias gdc='git diff --cached'
alias gb='git branch'
alias gba='git branch -a'
alias gcl='git clone'
alias gr='git rebase -i'
alias grc='git rebase --continue'
alias grsh='git reset --soft HEAD^'
alias grh='git reset --hard'
alias cio='git commit -c ORIG_HEAD'
alias gl='git log'
alias lg='git log --pretty=oneline'
alias gp='git push hpqdroid HEAD:refs/for/jb4.2_clvp_snapshot'
alias gp='git push hpq HEAD:refs/for/jb4.2_clvp_snapshot'
alias kw="(export VISUAL=: && git -c apply.whitespace=fix add -ue .) && git checkout . && git reset"
alias fr='find . |xargs grep --color -n'
alias frc='find . -name "*.c"|xargs  grep --color -n'
alias frh='find . -name "*.h"|xargs  grep --color -n'
alias glg="git log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"
alias gg="git log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr)%Creset' --abbrev-commit --date=relative"
alias gll='git log --graph --pretty=oneline --abbrev-commit'
alias ggs="gg --stat"
alias gsl="git shortlog -sn"
alias gw="git whatchanged"
alias gwho="git shortlog -s -n"
alias csk="astyle  --style=kr  -t -f -N -S -M -p -H -U -Y -k3"
alias csa="astyle  --style=ansi  -t -f -N -S -M -p -H -U -Y -k3"
alias kcs="astyle  --style=kr  -t  -N  -M -p -H -U -Y -k3"
alias clint="splint   +bounds  -formattype -warnposix -retvalint  -predboolint +matchanyintegral"
alias kfr='find .  |xargs | xjobs  -v 0  -j 11  grep'
alias kfrc='find . -name "*.c" |xargs | xjobs  -v 0  -j 11  grep'
alias kfrh='find . -name "*.h" |xargs | xjobs  -v 0  -j 11  grep'
alias ff='find . |grep -i '
alias cd2='cd ../../'
alias cd3='cd ../../../'
alias cd4='cd ../../../../'
alias cd4='cd ../../../../'
alias cd...="cd ../.."
alias cd....="cd ../../.."
alias cd.....="cd ../../../.."
alias cd......="cd ../../../../.."
alias cd-='popd'
alias cd--='popd -2'
alias cd---='popd -3'
alias d="dirs -v"
alias pd=pushd
alias pd1='pushd +1'
alias pd2='pushd +2'
alias pd3='pushd +3'
alias pd4='pushd +4'
alias pd5='pushd +5'
alias b=pd1
alias td='find .  -name .repo -prune -o -name .git -prune -o -mtime 0 '

function gtd()
{
    find .  -name .repo -prune -o -name .git -prune -o -mtime 0 -print0| xargs -0 grep --color -n "$@"
}

function jgrep()
{
    find . -name .repo -prune -o -name .git -prune -o  -type f -name "*\.java" -print0 | xargs -0 grep --color -n "$@"
}

function cgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -type f \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h' \) -print0 | xargs -0 grep --color -n "$@"
}

function hgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -type f \( -name '*.h' \) -print0 | xargs -0 grep --color -n "$@"
}

function xmlgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -type f \( -name '*.xml' \) -print0 | xargs -0 grep --color -n "$@"
}

function mkgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -type f \( -name '*.mk' \) -print0 | xargs -0 grep --color -n "$@"
}

function resgrep()
{
    for dir in `find . -name .repo -prune -o -name .git -prune -o -name res -type d`; do find $dir -type f -name '*\.xml' -print0 | xargs -0 grep --color -n "$@"; done;
}

function getkerneltop
{
    local TOPFILE=scripts/checkpatch.pl
    if [ -n "$KERNEL_DIR" -a -f "$TOP/$TOPFILE" ] ; then
        echo $KERNEL_DIR
    else
        if [ -f $TOPFILE ] ; then
            # The following circumlocution (repeated below as well) ensures
            # that we record the true directory name and not one that is
            # faked up with symlink names.
            PWD= /bin/pwd
        else
            # We redirect cd to /dev/null in case it's aliased to
            # a command that prints something as a side-effect
            # (like pushd)
            local HERE=$PWD
            local T=
            while [ \( ! \( -f $TOPFILE \) \) -a \( $PWD != "/" \) ]; do
                cd .. > /dev/null
                T=`PWD= /bin/pwd`
            done
            cd $HERE > /dev/null
            if [ -f "$T/$TOPFILE" ]; then
                echo $T
            fi
        fi
    fi
}

function pck()
{
	local  T=$(getkerneltop)
	git show "$@" |$T/scripts/checkpatch.pl --no-signoff - 
}

function gdk()
{
	local  T=$(getkerneltop)
	git diff "$@" |$T/scripts/checkpatch.pl --no-signoff - 
}


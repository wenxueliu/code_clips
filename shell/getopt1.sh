#!/bin/bash

trap ':' INT QUIT TERM PIPE HUP # sends execution to end tracing section

function usage {
	cat <<-END >&2
	USAGE: uprobe [-FhHsv] [-d secs] [-p PID] {-l target |
	              uprobe_definition [filter]}
	                 -F              # force. trace despite warnings.
	                 -d seconds      # trace duration, and use buffers
	                 -l target       # list functions from this executable
	                 -p PID          # PID to match on events
	                 -v              # view format file (don't trace)
	                 -H              # include column headers
	                 -s              # show user stack traces
	                 -h              # this usage message

END
	exit
}

function warn {
	if ! eval "$@"; then
		echo >&2 "WARNING: command failed \"$@\""
	fi
}

function die {
	echo >&2 "$@"
	exit 1
}

function edie {
	# die with a quiet end()
	echo >&2 "$@"
	exec >/dev/null 2>&1
	end
	exit 1
}

opt_duration=0; duration=; opt_pid=0; pid=; opt_filter=0; filter=
opt_view=0; opt_headers=0; opt_stack=0; dmesg=2; debug=0; opt_force=0
opt_list=0; target=
### process options
while getopts Fd:hHl:p:sv opt
do
	case $opt in
	F)	opt_force=1 ;;
	d)	opt_duration=1; duration=$OPTARG ;;
	p)	opt_pid=1; pid=$OPTARG ;;
	l)	opt_list=1; target=$OPTARG ;;
	H)	opt_headers=1 ;;
	s)	opt_stack=1 ;;
	v)	opt_view=1 ;;
	h|?)	usage ;;
	esac
done
shift $(( $OPTIND - 1 ))
uprobe=$1
shift
if (( $# )); then
	opt_filter=1
	filter=$1
fi

echo opt_duration=$opt_duration
echo duration=$duration
echo opt_pid=$opt_pid
echo pid=$pid
echo target=$target
echo opt_headers=$opt_headers
echo opt_stack=$opt_stack
echo opt_view=$opt_view
echo opt_force=$opt_force
echo opt_filter=$opt_filter
echo filter=$filter

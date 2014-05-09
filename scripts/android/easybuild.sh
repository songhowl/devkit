#!/bin/bash
echo $OPTIND
# select the correct version java, java-6-sun
export JAVA_HOME=/usr/lib/jvm/java-6-sun
export PATH=$JAVA_HOME/bin:$PATH


product=cypress
boardtype=PROTO
buildvariant=eng
job_number=$(cat /proc/cpuinfo | grep processor | wc -l)

while getopts ":b:p:v:j:" opt; do
  case $opt in
    b)
      
      boardtype=$OPTARG
      ;;
    p)
	echo "+++++set product\n"
      product=$OPTARG
      ;;
    v)
      buildvariant=$OPTARG
      ;;
    j)
      job_number=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

shift `expr $OPTIND - 1`
action=$1

if test "x$action" = x; then
        action=all
fi

echo "action: $action"
echo "product: $product"
echo "boardtype: $boardtype"

#!/bin/bash

if test -z "$1"
then
  echo "usage: gitmerge.sh <branch>"
  exit
fi

## COPY USER FILES INTO TEMP FILES ##
for i in user/Makefile.user user/UserLinkdef.h src/user/User.hxx src/user/User.cxx
do cat $i > ${i}--TEMP
done

## MERGE ##
git merge --no-commit $1

## REVERT USER FILES & REMOVE TEMP FILES ##
for i in user/Makefile.user user/UserLinkdef.h src/user/User.hxx src/user/User.cxx
do cat ${i}--TEMP > $i ; rm -f ${i}--TEMP
done


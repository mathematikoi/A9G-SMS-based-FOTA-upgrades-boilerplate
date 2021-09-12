#/bin/bash
rm -rf $GPRS_PROJ_ROOT/closterbonebase
cp -R . $GPRS_PROJ_ROOT/closterbonebase
cd $GPRS_PROJ_ROOT &&
    ./build.sh clean all &&
    ./build.sh closterbonebase &&
    cd $OLDPWD &&
    cp -R $GPRS_PROJ_ROOT/hex/closterbonebase/ build

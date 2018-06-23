diff /usr/src/app/arduino.ino /data/arduino.ino || PROGRAMMER=1
if [ "${PROGRAMMER:-}" == "1" ]; then
  echo $PROGRAMMER
  pushd /usr/src/app
  make upload && cp arduino.ino /data/
  unset PROGRAMMER
  popd
fi

node server.js
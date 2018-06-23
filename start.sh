diff /usr/src/app/arduino/arduino.ino /data/arduino/arduino.ino || PROGRAMMER=1
if [ "${PROGRAMMER:-}" == "1" ]; then
  echo $PROGRAMMER
  pushd /usr/src/app/arduino
  make upload && cp arduino.ino /data/
  unset PROGRAMMER
  popd
fi

node /app/server.js
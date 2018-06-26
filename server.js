var http = require('http').createServer(handler); //require http server, and create server with function handler()
var fs = require('fs'); //require filesystem module
var io = require('socket.io')(http) //require socket.io module and pass the http object (server)
var Gpio = require('onoff').Gpio; //include onoff to interact with the GPIO
var LED = new Gpio(4, 'out'); //use GPIO pin 4 as output
// var pushButton = new Gpio(17, 'in', 'both'); //use GPIO pin 17 as input, and 'both' button presses, and releases should be handled
var SerialPort = require('serialport');

http.listen(80); //listen to port 80


var port = new SerialPort('/dev/ttyACM0', function (err) {
  if (err) {
    return console.log('Error: ', err.message);
  }
  console.log("Serial Port Successflly Initialised!");
});

function handler (req, res) { //create server
  fs.readFile('/usr/src/app/index.html', function(err, data) { //read file index.html in public folder
    if (err) {
      res.writeHead(404, {'Content-Type': 'text/html'}); //display 404 on error
      return res.end("404 Not Found Here!");
    }
    res.writeHead(200, {'Content-Type': 'text/html'}); //write HTML
    res.write(data); //write data from index.html
    return res.end();
  });
}

io.sockets.on('connection', function (socket) {// WebSocket Connection

  socket.on('light', function(data) { //get light switch status from client
    lightvalue = data;
    if (lightvalue != LED.readSync()) { //only change LED if status has changed
      LED.writeSync(lightvalue); //turn LED on or off
    }
  });
});

io.sockets.on('connection', function (matrix_socket) {// WebSocket Connection
  
  matrix_socket.on('message', function(data) { //get light switch status from client

      port.write('data');
      console.log("Data: " + data);
  });
});

process.on('SIGINT', function () { //on ctrl+c
  LED.writeSync(0); // Turn LED off
  LED.unexport(); // Unexport LED GPIO to free resources
  pushButton.unexport(); // Unexport Button GPIO to free resources
  process.exit(); //exit completely
}); 
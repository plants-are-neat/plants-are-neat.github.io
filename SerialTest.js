//Globals
var channelID = 1226802;  //Channel ID for thingspeak
var baudRate = 9600;  //Baud Rate for the serial connection

//External libraries **IMPORTANT** remember to use npm install X on command terminal to install all libraries i.e. npm install thingspeakclient
var ThingSpeakClient = require('thingspeakclient'); //Communication for thingspeak
const SerialPort = require('serialport');   //Serial connection between board **IMPORTANT** Only one serial connection is allowed
                                            //Having the serial monitor open on arduino WILL cause one of the two to fail if
                                            //ran simultaneously 
const prompt = require('prompt-sync')();    //For prompting the user for com port



//Instances for libraries
let Readline = SerialPort.parsers.Readline; // make instance of Readline parser
var client = new ThingSpeakClient();  //Make instance of thingspeak client
//var myPort = new SerialPort('COM6', baudRate);//Make instance of thingspeak client **IMPORTANT** the com6 is a placeholder, value gets replaced
let parser = new Readline(); // make a new parser to read ASCII lines
client.attachChannel(channelID, { writeKey:'9M50MDHK1RPV9KW8'});  //Attaches our thingspeak channel to the client



//Internal variables
var Mport;  //port to be used
var ans;    //user answer for com prompt
var count = 1;



parser.on('data', readSerialData);

function readSerialData(data) {
  try{
  console.log(data);                                                              //Reads the data to the user
  var temp = data.split(',');                                                     //All comma separated so split at each comma
  client.updateChannel(channelID, {field1: temp[0], field2: temp[1], field3: temp[2], field4: temp[3], field5: temp[4], field6: temp[5]}, function(err, resp) {
    if (!err && resp > 0) {
        console.log('update successfully. Entry number was: ' + resp);
    }
    else
      console.log("oopies");
  }).catch(e => {console.log(e)});
}
  catch (error) {
    console.log("Error reading serial");
  }
                                                                                  //Update the thingspeak channel
  
}



// Promise approach
SerialPort.list().then(ports => {
  try{
  ports.forEach(function(port) {
    console.log(count + " : " + port.path + " : " + port.pnpId + " : " + port.manufacturer);  //Displays all ports attatched to computer
    count = count +1;
  });
  ans = prompt("Select the node port: ")                    //Prompt user for node
  Mport = ports[parseInt(ans) - 1].path;                                         //Save correct port, index starts at 0 but index shown to user starts at 1
  myPort = new SerialPort(Mport, baudRate);                       //New serial port instance, causes an error that can be ignored
  myPort.pipe(parser); // pipe the serial stream to the parser
}
catch (error)
  {
    console.log("Error on init")
  }
}).catch(e => {console.log(e)});


/* 
 *Test script, created from GitHub
 * Add comment to tet GitHub integration with mSB.com
 * Add new comment
 */

var timerEvent; // In case you use a timer for fetching data
var self; 
var exports = module.exports = {
    
    // The Start method is called from the Host. This is where you 
    // implement your code to fetch the data and submit the message
    // back to the host.
    Start : function () {
        self = this;
        // Debug command can be used as console.log and 
        // will be vissible in the portal if you enable
        // Debug on the node
        self.Debug('The Start method is called.');

        /*
        self.AddNpmPackage('npmPackage', true, function(err){
            if(!err){
                // This is your startpoint...
                packageVariable = require('npmPackage');
            }
            else{       
                self.ThrowError(null, '00001', 'Unable to install the npmPackage npm package');
                return;     
            }
        });
        */

        // The timer event can be used for creating message on a 
        // scheduled interval. In this case every 10 seconds.
        timerEvent = setInterval(function () {
            // TO DO! 
            // This is where you add code to read a sensor
            // and create a payload message.
            var payload = {
                someRandomValue : Math.random() 
            };

            // Submit payload to the next service
            self.SubmitMessage(payload);
        }, 10000);
    },

    // The Stop method is called from the Host when the Host is 
    // either stopped or has updated integrations. 
    Stop : function () {
        self.Debug('The Stop method is called.');
        // Stop the timerEvent
        clearInterval(timerEvent);
    },    
    
    // The Process method is called from the orchestration engine as it receives 
    // messages from other services. The [messasge] parameter is a JSON 
    // object (the payload) and the [context] parameter is a 
    // value/pair object with parameters provided by the portal.
    Process : function (message, context) {
        // This is where you can manipulate the message
        
        message.state = "It's working";
        
        self.SubmitMessage(payload);

    },    
} 

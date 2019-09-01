var connectStatus = "Disconnected"
function startConnect() {
	clientID = "clientID-" + parseInt(Math.random() * 100);
	host = document.getElementById("host").value;
    port = document.getElementById("port").value;
	client = new Paho.MQTT.Client(host, Number(port), clientID);
	// set callback handlers
	client.onConnectionLost = onConnectionLost;
	client.onMessageArrived = onMessageArrived;

	// connect the client
	client.connect({onSuccess:onConnect});

}
// called when the client connects
function onConnect() {
	connectStatus = "Connected"
	document.getElementById("connection_status").innerHTML = connectStatus
	//client.subscribe();
	
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
	console.log("onMessageArrived:"+message.payloadString);
}
	
function startDisconnect() {
    client.disconnect();
	connectStatus = "Disconnected"
	document.getElementById("connection_status").innerHTML = connectStatus
}
function turnOn(topic) {
	console.log(topic)
	document.getElementById(topic).innerHTML = 1;	
	if (connectStatus=="Disconnected") {
		alert("Client is Disconnected")
	}
	else {
		message = new Paho.MQTT.Message("{'state':1}");
		message.destinationName = topic;
		client.send(message);
	}
}

function turnOff(topic) {
	console.log(topic)
	if (connectStatus=="Disconnected") {
		alert("Client is Disconnected")
	}
	else {
		client.subscribe(topic)
		document.getElementById(topic).innerHTML = 0;
		message = new Paho.MQTT.Message("{'state':0}");
		message.destinationName = topic;
		client.send(message);
	}
}

function changeTab(nodeID) {
	console.log(nodeID);
	$('#devices').empty();
	$.post("http://localhost:5000/getDevices",`{"nodeID":"${nodeID}"}`, function(content){
		for (var i=0; i<content.length; i++){
			let deviceName = content[i].name;
			let deviceStatus = content[i].status;
			let deviceLocation = content[i].location;
			$("#devices").append(`<p> ${deviceName} <span id=${deviceLocation}> ${deviceStatus} </span> <input type="button" onclick="turnOn('${deviceLocation}')" value="ON">&nbsp;<input type="button" onclick="turnOff('${deviceLocation}')" value="OFF"></p>`);
		};
	});
	};


$(document).ready(function() {
	$.get("http://localhost:5000/getNodes", function(content) {
		for (var i=0; i<content.length; i++){
			let nodeName = content[i].name;
			let nodeID = content[i].nodeID;
			let numberOfDevices = content[i].numberOfDevices;
			let nodeLocation = content[i].location;
			$('#nodes').append(`<button class="nodetabs" onclick="changeTab('${nodeID}')" id='${nodeLocation}'>${nodeName}</button>`);
			
		};
	});

	$.post("http://localhost:5000/getDevices",'{"nodeID":"node01"}', function(content){
		for (var i=0; i<content.length; i++){
			let deviceName = content[i].name;
			let deviceStatus = content[i].status;
			let deviceLocation = content[i].location;
			$("#devices").append(`<p> ${deviceName} <span id=${deviceLocation}> ${deviceStatus} </span> <input type="button" onclick="turnOn('${deviceLocation}')" value="ON">&nbsp;<input type="button" onclick="turnOff('${deviceLocation}')" value="OFF"></p>`);
		};
	});
	
	$('#register_node_button').click(function(event) {
		document.location.href = 'registerNode.html';
	});
	
	$('#register_device_button').click(function(event) {
		document.location.href = 'registerDevice.html';
	});
	
	
	$("#submitBtn").click(function(){        
		$.post("http://localhost:5000/node/register", $("#node-form").serialize(), function(data) {
			alert("form submitted");
			console.log(data);
			document.location.href = 'app.html';
		});
	});
	
	$("#submitBtn2").click(function(){        
		$.post("http://localhost:5000/device/register", $("#device-form").serialize(), function(data) {
			alert("form submitted");
			console.log(data);
			document.location.href = 'app.html';
		});
	});
	
	
});



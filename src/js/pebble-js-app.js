var initialized = false;
var options     = {
  "username": "cha0s",
};

var xhrRequest = function (url, type, callback) {

  var xhr    = new XMLHttpRequest();
  xhr.onload = function () {
    console.log ("Executing callback.");
    callback(this.responseText);
  };
  xhr.addEventListener("error", xhrRequestError, false);
  xhr.addEventListener("abort", xhrRequestCanceled, false);

  xhr.onreadystatechange = function (oEvent) {
      if (xhr.readyState === 4) {
          if (xhr.status === 200) {
            console.log(xhr.responseText)
          } else {
             console.log("Error", xhr.statusText);
          }
      }
  };

  xhr.ontimeout = function () {
    console.log ("WARNING: timeout!");
  }

  console.log ("Opening request.");
  xhr.open(type, url);
  console.log ("Sending request.");
  xhr.send();
};
function xhrRequestError (e) {
  console.log("Error loading pr0gramm profile.");
}
function xhrRequestCanceled (e) {
  console.log("Canceled loading pr0gramm profile.");
}

function p0grammGet() {

  // Construct URL
  var url = 'https://pr0gramm.com/api/profile/info?name='+ options.username;

  console.log("Fetching url: "+ url);

  // Send request to pr0gramm.com
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with user info
      var json = JSON.parse(responseText);

      // get username
      var username = json.user.name;
      console.log("Username is " + username);

      // get benis
      var benis = json.user.score;
      console.log("Benis is " + benis);

      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_USERNAME": username,
        "KEY_BENIS": benis
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("pr0gramm info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending pr0gramm info to Pebble!");
        }
      );
    }
  );
}

function p0grammError(err) {
  console.log("Error requesting p0gramm!");
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");
    initialized = true;
    var json = window.localStorage.getItem('pr0gramm-config');
    if (typeof json === 'string') {
      try {
        options = JSON.parse(json);
        console.log("Loaded stored config: " + json);

        // Get the initial pr0gramm info
        p0grammGet();
      } catch(e) {
        console.log("stored config json parse error: " + json + ' - ' + e);
      }
    }
});

Pebble.addEventListener("showConfiguration", function() {
  var config_param = '?configuration=' + encodeURI(JSON.stringify(options));
  try {
    var watch = Pebble.getActiveWatchInfo();
  } catch(e) {
    console.log("getActiveWatchInfo error: " + e);
    var watch = null;
  }
  var platform = "&platform=" + ((watch != null) ? watch.platform : "unknown");
  console.log("showing configuration");
  Pebble.openURL("https://w0rscht.menkent.uberspace.de/" + config_param + platform);
});

Pebble.addEventListener("webviewclosed", function(e) {
  var response = decodeURIComponent(e.response);
  try {
    options = JSON.parse(response);

    window.localStorage.setItem('pr0gramm-config', response);

    p0grammGet();
  } catch(e) {
    console.log("Response config json parse error: " + response + ' - ' + e);
  }
});

// Listen for when an AppMessage is received
Pebble.addEventListener("appmessage",
  function(e) {
    console.log("Appmessage received: "+ JSON.stringify(e.payload));
    p0grammGet();
  }
);

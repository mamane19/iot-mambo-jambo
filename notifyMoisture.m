% Read the soil moisture channel data from the past two weeks.
% Send an email and tell the user to add water if the value
% is in the lowest 10 %.

% Store the channel ID for the moisture sensor channel.
channelID = "CHANNEL_ID";

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = "ALERT_API_KEY";

% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf("Hello! Irrigation action needed!");

% Read the recent data.
moistureData = thingSpeakRead(channelID,'NumPoints',1,'Fields',2);

% Check to make sure the data was read correctly from the channel.
if isempty(moistureData)
    alertBody = 'No data found!';
else
    lastValue = moistureData(end);
    dangerLevelHigh = 620;
    dangerLevelLow = 220;

    % Set the outgoing message
    if lastValue > dangerLevelHigh
        alertBody = sprintf('Moisture level is %0.2f. Please discontinue irrigation.', lastValue);
    elseif (lastValue < dangerLevelLow)
        alertBody = sprintf('Moisture level is %0.2f. Please start irrigation.', lastValue);
    end
end

 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);
catch someException
    fprintf("Failed to send: %s\n", someException.message);
end
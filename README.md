# mmlogmonitor
Mattermost Log Monitor (just couldnt stom myself)

So after finishing the NextCloud to MatterMost bot gateway. I
just could stop myself from writing a MatterMost Log monitor
so I will write a daemon that will monitor a log that it is 
directed to and monitor it for events. Apply a simple filter
so we only get the events we want and then send these
events to our MatterMost WebHook. Seems simple enough.

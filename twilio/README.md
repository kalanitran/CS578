https://www.twilio.com/docs/messaging/tutorials/how-to-send-sms-messages/cpp

```
account_sid=ACXXXXXXXXXXXXXXXXXXXXXX
auth_token=your_auth_token
bin/cpp_demo -a $account_sid -s $auth_token -t "+18005551212" -f "+18005551213" -m "Hello, World!"
# Optionally, use '-p http://some/path/to/img.jpg' to send an MMS.
```

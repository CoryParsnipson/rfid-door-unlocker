RFID Door Unlocker
==================

Yes, I'm making one too. It's an RFID card reading door unlocker. (No, it doesn't move the door, it just undoes the deadbolt.)

### Notes

1. This is an arduino project.
2. This uses the MFRC522 arduino rfid reader shield and library (https://github.com/miguelbalboa/rfid)
3. This also uses a (HS-55) cheap RC servo
4. I have a TMB12A05 (passive piezo buzzer) that beeps whenever a card is read.

### TODO

* Probably need to swap out the MFRC522 for a different freq reader. Need more range too?
* Need to add RSA key or something to authenticate RFID PICC
* Need to find way to mount servo to the deadbolt
* Probably need to add "disengage" on servo to deadbolt so people can come in manually with key (in case landlord needs to come in or if the batteries die)
* Come up with housing + door mount
* Find 5 - 7V battery casing
* Come up with a way to get notifications somehow when the batteries are low?

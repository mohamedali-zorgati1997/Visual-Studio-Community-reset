# Visual-Studio-Community-reset
An easy way to get extra 30 days for Visual Studio Community Edition.

Visual Studio Community Edition is supposed to be free and yet it works only for 30 days and then asks you to create an account and to be honest I am kind of lazy and hate sharing my e-mail and details. 
So after a little search in the internet, I found the necessary registry key to edit and how to edited.

You will need to link the Crypt32.lib as I needed the CryptProtectData and CryptUnprotectData functions (you may also need to add the Crypt32.dll found on System32 folder)

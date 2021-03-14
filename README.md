# Menu

[![sampctl](https://img.shields.io/badge/sampctl-fMenu-2f2f2f.svg?style=for-the-badge)](https://github.com/roesanne26/Menu)

![](https://media.discordapp.net/attachments/759415958937796649/768101257788653578/unknown.png)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

## Installation

Simply install to your project:

```bash
sampctl package install roesanne26/menu
```

Include in your code and begin using the library:

```pawn
#include <menu>
```
## Functions
```pawn
Menu_Add(playerid, const item[26]);
Menu_Show(playerid, menuid, const tittle[] = "Menu", const header[] = "Interaction");
Menu_Hide(playerid);
```
## Usage

```pawn
#include <Pawn.CMD>
#include <menu>

CMD:ya(playerid, params[])
{
    for(new str[26], i = 0; i < strval(params) + 1; i ++)
    {
        format(str, 26, "Item %d", i);
        Menu_Add(playerid, str);
    }
    TogglePlayerControllable(playerid, 0);
    Menu_Show(playerid, 1, "HELLO YOOY!", "Interaction");
    return 1;
} 

public OnMenuResponse(playerid, menuid, response, listitem)
{
    new 
        str[128];
    
    format(str, sizeof(str), "Menu=%d, Response=%d, Listitem=%d", menuid, response, listitem);
    SendClientMessage(playerid, -1, str);
    if(!response)
    {
        Menu_Hide(playerid);
        TogglePlayerControllable(playerid, 1);
    }
    return 1;
}
```

To test, simply run the package:

```bash
sampctl package run
```
## Credits
* Me

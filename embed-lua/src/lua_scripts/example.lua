a  = 7 + 11
a  = a + 100

function AddStuff(a, b)
    print("[LUA] AddStuff("..a..",  "..b..")called \n")
    return a  + b;
end


PlayerTitle = "Squire"
PlayerName = "Ciaran"
PlayerFamily = "Wirral"
PalyerLevel = 20

player = { Title = "Squire", Name = "Ciaran", Family = "Wirral", Level = 30}


players = {}
players[0] = { Title = "Squire", Name = "Ciaran", Family = "Wirral", Level = 30}
players[1] = { Title = "Lord", Name = "Diego", Family = "Brazil", Level = 50}

function GetPlayer(n)
    return player[n]
end


function DoAThing(a, b)
    print("[LUA] DoAThing("..a..","..b..") called \n")
    c = HostCFunction(a + 10, b)
    return c
end


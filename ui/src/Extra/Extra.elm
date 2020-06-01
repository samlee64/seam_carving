module Extra.Extra exposing (..)


ternary : a -> a -> Bool -> a
ternary item1 item2 bool =
    if bool then
        item1

    else
        item2

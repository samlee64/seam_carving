module Extra.Cmd exposing (..)


none : model -> ( model, Cmd msg )
none m =
    ( m, Cmd.none )


cmd : (model -> Cmd msg) -> model -> ( model, Cmd msg )
cmd cmdMsg m =
    ( m, cmdMsg m )

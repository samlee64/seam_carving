module Data.Mouse exposing (..)

import Json.Decode exposing (..)


type alias MouseMoveData =
    { offsetX : Int
    , offsetY : Int
    , offsetHeight : Float
    , offsetWidth : Float
    }


mouseMoveDataDecoder : Decoder MouseMoveData
mouseMoveDataDecoder =
    map4 MouseMoveData
        (at [ "offsetX" ] int)
        (at [ "offsetY" ] int)
        (at [ "target", "offsetHeight" ] float)
        (at [ "target", "offsetWidth" ] float)

module Data.Markings exposing (..)

import Json.Decode exposing (..)
import Json.Encode as E


type alias Markings =
    { destroy : List Int, protect : List Int }


markingsDecoder : Decoder Markings
markingsDecoder =
    map2 Markings
        (at [ "detail", "destroy" ] (list int))
        (at [ "detail", "protect" ] (list int))


encodeMarkings : Markings -> E.Value
encodeMarkings markings =
    E.object
        [ ( "destroy", E.list E.int markings.destroy )
        , ( "protect", E.list E.int markings.protect )
        ]

module Data.Markings exposing (..)

import Json.Decode exposing (..)
import Json.Encode as E


type alias Markings =
    { destroy : List Int, protect : List Int, imageHeight : Int, imageWidth : Int }


markingsDecoder : Decoder Markings
markingsDecoder =
    map4 Markings
        (field "destroy" (list int))
        (field "protect" (list int))
        (field "imageHeight" int)
        (field "imageWidth" int)


encode : Markings -> E.Value
encode markings =
    E.object
        [ ( "destroy", E.list E.int markings.destroy )
        , ( "protect", E.list E.int markings.protect )
        , ( "imageHeight", E.int markings.imageHeight )
        , ( "imageWidth", E.int markings.imageWidth )
        ]

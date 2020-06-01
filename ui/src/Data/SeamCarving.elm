module Data.SeamCarving exposing (..)

import Json.Decode exposing (..)
import Json.Encode as E


type alias GrowImageParams =
    { imageName : String
    , showIntermediateSteps : Bool
    , numSteps : Int
    , addHeight : Int
    , addWidth : Int
    }


type alias GrowImageResp =
    { imageName : String
    , showIntermediateSteps : Bool
    , numSteps : Int
    , addHeight : Int
    , addWidth : Int
    }


type alias PollStatusParams =
    { imageName : String
    }


type alias PollStatusResp =
    { status : String }


encodeGrowImageParams : GrowImageParams -> E.Value
encodeGrowImageParams params =
    E.object
        [ ( "imageName", E.string params.imageName )
        , ( "showIntermediateSteps", E.bool params.showIntermediateSteps )
        , ( "numSteps", E.int params.numSteps )
        , ( "addWidth", E.int params.addWidth )
        , ( "addHeight", E.int params.addHeight )
        ]


encodePollParams : PollStatusParams -> E.Value
encodePollParams params =
    E.object [ ( "imageName", E.string params.imageName ) ]


growImageRespDecoder : Decoder GrowImageResp
growImageRespDecoder =
    map5 GrowImageResp
        (field "imageName" string)
        (field "showIntermediateSteps" bool)
        (field "numSteps" int)
        (field "addHeight" int)
        (field "addWidth" int)


pollRespDecoder : Decoder PollStatusResp
pollRespDecoder =
    map PollStatusResp (field "status" string)

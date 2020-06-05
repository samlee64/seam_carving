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
    , executionId : String
    }


type alias ContentAmplificationParams =
    { imageName : String
    , showIntermediateSteps : Bool
    , factor : Int
    }


type alias ContentAmplificationResp =
    { executionId : String }


type alias ExecutionStatusParams =
    { executionId : String
    }


type alias ExecutionStatusResp =
    { status : Status }


type Status
    = Executing
    | Uploading
    | Done
    | Error


stringToStatus : String -> Maybe Status
stringToStatus input =
    case input of
        "executing" ->
            Just Executing

        "uploading" ->
            Just Uploading

        "done" ->
            Just Done

        "error" ->
            Just Error

        _ ->
            Nothing


statusToString : Status -> String
statusToString status =
    case status of
        Executing ->
            "executing"

        Uploading ->
            "uploading"

        Done ->
            "done"

        Error ->
            "error"


encodeGrowImageParams : GrowImageParams -> E.Value
encodeGrowImageParams params =
    E.object
        [ ( "imageName", E.string params.imageName )
        , ( "showIntermediateSteps", E.bool params.showIntermediateSteps )
        , ( "numSteps", E.int params.numSteps )
        , ( "addWidth", E.int params.addWidth )
        , ( "addHeight", E.int params.addHeight )
        ]


growImageRespDecoder : Decoder GrowImageResp
growImageRespDecoder =
    map6 GrowImageResp
        (field "imageName" string)
        (field "showIntermediateSteps" bool)
        (field "numSteps" int)
        (field "addHeight" int)
        (field "addWidth" int)
        (field "executionId" string)


encodeContentAmplificationParams : ContentAmplificationParams -> E.Value
encodeContentAmplificationParams params =
    E.object
        [ ( "imageName", E.string params.imageName )
        , ( "showIntermediateSteps", E.bool params.showIntermediateSteps )
        , ( "factor", E.int params.factor )
        ]


contentAmplificationRespDecoder : Decoder ContentAmplificationResp
contentAmplificationRespDecoder =
    map ContentAmplificationResp (field "executionId" string)


statusDecoder : Decoder Status
statusDecoder =
    string
        |> andThen
            (\str ->
                stringToStatus str
                    |> Maybe.map (\status -> succeed status)
                    |> Maybe.withDefault (fail (str ++ " is not a valid status type"))
            )


pollRespDecoder : Decoder ExecutionStatusResp
pollRespDecoder =
    map ExecutionStatusResp (field "status" statusDecoder)

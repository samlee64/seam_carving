module Data.SeamCarving exposing (..)

import Data.Markings as Markings exposing (Markings)
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
    , factor : Float
    }


type alias ContentAmplificationResp =
    { executionId : String }


type alias RemoveObjectParams =
    { imageName : String
    , showIntermediateSteps : Bool
    , lockRatio : Bool
    , onlyHorizontal : Bool
    , onlyVertical : Bool
    , markings : Markings
    , imageWidth : Int
    , imageHeight : Int
    }


type alias RemoveObjectResp =
    { executionId : String }


type alias ExecutionStatusParams =
    { executionId : String }


type alias ExecutionStatusResp =
    { status : Status
    , id : String
    , imageName : String
    , routine : String
    , s3Url : Maybe (List String)
    }


type alias InputImagesResp =
    { keys : List String
    , bucket : String
    }


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
        , ( "factor", E.float params.factor )
        ]


contentAmplificationRespDecoder : Decoder ContentAmplificationResp
contentAmplificationRespDecoder =
    map ContentAmplificationResp (field "executionId" string)


encodeRemoveObjectParams : RemoveObjectParams -> E.Value
encodeRemoveObjectParams params =
    E.object
        [ ( "imageName", E.string params.imageName )
        , ( "showIntermediateSteps", E.bool params.showIntermediateSteps )
        , ( "lockRatio", E.bool params.lockRatio )
        , ( "onlyHorizontal", E.bool params.onlyHorizontal )
        , ( "onlyVertical", E.bool params.onlyVertical )
        , ( "markings", Markings.encode params.markings )
        , ( "imageHeight", E.int params.imageHeight )
        , ( "imageWidth", E.int params.imageWidth )
        ]


removeObjectRespDecoder : Decoder RemoveObjectResp
removeObjectRespDecoder =
    map RemoveObjectResp (field "executionId" string)


removeObjectRespEventDecoder : Decoder RemoveObjectResp
removeObjectRespEventDecoder =
    map RemoveObjectResp (at [ "detail", "executionId" ] string)


inputImageRespDecoder : Decoder InputImagesResp
inputImageRespDecoder =
    map2 InputImagesResp
        (field "keys" (list string))
        (field "bucket" string)


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
    map5 ExecutionStatusResp
        (field "status" statusDecoder)
        (field "id" string)
        (field "imageName" string)
        (field "routine" string)
        (field "s3Url" (nullable (list string)))

module Request.SeamCarving exposing (..)

import Bytes exposing (Bytes)
import Bytes.Decode
import Data.SeamCarving exposing (..)
import Flags exposing (Flags)
import Http
import Image exposing (Image)
import Json.Decode exposing (..)
import RemoteData as RD exposing (WebData)
import Request.Request exposing (..)


growImage : Flags -> (WebData GrowImageResp -> msg) -> GrowImageParams -> Cmd msg
growImage flags msg params =
    let
        body =
            encodeGrowImageParams params
    in
    post flags [ "seam", "grow" ]
        |> withJsonBody body
        |> withExpect growImageRespDecoder msg
        |> request


contentAmplification : Flags -> (WebData ContentAmplificationResp -> msg) -> ContentAmplificationParams -> Cmd msg
contentAmplification flags msg params =
    let
        body =
            encodeContentAmplificationParams params
    in
    post flags [ "seam", "content-amplification" ]
        |> withJsonBody body
        |> withExpect contentAmplificationRespDecoder msg
        |> request


removeObject : Flags -> (WebData RemoveObjectResp -> msg) -> RemoveObjectParams -> Cmd msg
removeObject flags msg params =
    let
        body =
            encodeRemoveObjectParams params
    in
    post flags [ "seam", "remove-object" ]
        |> withJsonBody body
        |> withExpect removeObjectRespDecoder msg
        |> request


getExecutionStatus : Flags -> (WebData ExecutionStatusResp -> msg) -> ExecutionStatusParams -> Cmd msg
getExecutionStatus flags msg params =
    let
        queryString =
            "?executionId=" ++ params.executionId
    in
    get flags [ "seam", "status", queryString ]
        |> withExpect pollRespDecoder msg
        |> request

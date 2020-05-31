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


getImageLinks : Flags -> (WebData (List String) -> msg) -> Cmd msg
getImageLinks flags msg =
    let
        path =
            "files"
    in
    get flags [ path ]
        |> withExpect (list string) msg
        |> request


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

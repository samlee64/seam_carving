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


carveImage : Flags -> (WebData CarveImageResp -> msg) -> CarveImageParams -> Cmd msg
carveImage flags msg params =
    let
        path =
            "seam"
    in
    get flags [ path ]
        |> withExpect carveImageRespDecoder msg
        |> request


getImageLinks : Flags -> (WebData (List String) -> msg) -> Cmd msg
getImageLinks flags msg =
    let
        path =
            "files"
    in
    get flags [ path ]
        |> withExpect (list string) msg
        |> request

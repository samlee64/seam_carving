module Request.SeamCarving exposing (..)

import Data.SeamCarving exposing (..)
import Flags exposing (Flags)
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

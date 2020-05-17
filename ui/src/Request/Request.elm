module Request.Request exposing (..)

import Http exposing (Request)
import HttpBuilder as HB
import Json.Decode as Decode
import Json.Encode as Encode
import Url.Builder as UrlBuilder


get : Flags -> List String -> Http.Request
get flags path =
    HB.crossOrigin flags.api path
        |> HB.get
        |> HB.withExpect Http.expectJson
        |> HB.request


post : Flags -> List String -> Http Request
post flags path =
    HB.crossOrigin flags.api path
        |> HB.post

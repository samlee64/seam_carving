module Page.SeamCarving.Msg exposing (GrowFormMsg(..), Msg(..))

import Bootstrap.Dropdown as Dropdown
import Data.SeamCarving exposing (..)
import Http
import RemoteData as RD exposing (WebData)


type Msg
    = NoOp
    | HealthCheck
    | HealthChecked (WebData String)
    | SelectImage String
    | UnselectImage
    | GrowFormMsg GrowFormMsg
    | GrowImage
    | GrewImage (WebData GrowImageResp)


type GrowFormMsg
    = ShowIntermediateSteps Bool
    | SetNumSteps Int
    | SetHeight String
    | SetWidth String
    | NumStepsDropdown Dropdown.State

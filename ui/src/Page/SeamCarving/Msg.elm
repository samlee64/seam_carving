module Page.SeamCarving.Msg exposing (..)

import Bootstrap.Dropdown as Dropdown
import Data.SeamCarving exposing (..)
import Http
import RemoteData as RD exposing (WebData)
import Time


type Msg
    = NoOp
    | HealthCheck
    | HealthChecked (WebData String)
    | SelectImage String
    | UnselectImage
    | GrowFormMsg GrowFormMsg
    | GrowImage
    | GrewImage (WebData GrowImageResp)
    | Tick Time.Posix
    | PolledExecutionStatus (WebData ExecutionStatusResp)
    | ContentAmplificationFormMsg ContentAmplificationFormMsg
    | AmplifyImage
    | AmplifiedImage (WebData ContentAmplificationResp)


type GrowFormMsg
    = ShowGrowIntermediateSteps Bool
    | SetNumSteps Int
    | SetHeight String
    | SetWidth String
    | NumStepsDropdown Dropdown.State


type ContentAmplificationFormMsg
    = ShowContentAmplificationIntermediateSteps Bool
    | SetFactor Int
    | FactorDropdown Dropdown.State

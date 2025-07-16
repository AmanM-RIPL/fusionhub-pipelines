// return type from fhdev_service api
export type FHDevReturn<T> = {
  Status: string,
  Message: string,
  data: T
}

export type ControllerResult<T = {}> = {
  Status: string,
  Message: string,
  result?: T
}

export type Payload = {
  username: string,
  userId: number,
}
